open Belt;

open Json.Decode;

module Resource = {
  type t('a) = {
    id: Model.id,
    attributes: option('a),
  };
  let decode = (decoder: Json.Decode.decoder('a), json: Js.Json.t) : t('a) => {
    id: json |> field("id", int),
    attributes: json |> optional(field("attributes", decoder)),
  };
};

module Document = {
  type meta = {
    totalPages: int,
    totalEntries: int,
  };
  type one('a) = {data: Resource.t('a)};
  type many('a) = {
    data: list(Resource.t('a)),
    meta,
  };
  type decodedMany('a) = {
    meta,
    resources: list('a),
  };
  let metaDecoder = (json: Js.Json.t) : meta => {
    totalPages: json |> field("total_pages", int),
    totalEntries: json |> field("total_entries", int),
  };
  let decodeOne =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : one('a) => {
    data: json |> field("data", resourceDecoder),
  };
  let decodeMany =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : many('a) => {
    meta: json |> field("meta", metaDecoder),
    data: json |> field("data", list(resourceDecoder)),
  };
};

module Error = {
  type t = {
    code: string,
    title: string,
  };
  let decodeOne = (json: Js.Json.t) => {
    code: json |> field("code", string),
    title: json |> field("title", string),
  };
  let decodeMany = (json: Js.Json.t) =>
    json |> field("errors", list(decodeOne));
};

module type Decodable = {
  type t;
  type attributes;
  let attributesDecoder: Js.Json.t => attributes;
  let resourceToRecord: Resource.t(attributes) => t;
};

module type Decoder = {
  type t;
  let decodeOne: Js.Json.t => t;
  let decodeMany: Js.Json.t => Document.decodedMany(t);
};

module MakeDecoder =
       (Decodable: Decodable)
       : (Decoder with type t := Decodable.t) => {
  type t = Decodable.t;
  let resourceDecoder = Resource.decode(Decodable.attributesDecoder);
  let decodeOne = (json: Js.Json.t) : t =>
    Document.decodeOne(resourceDecoder, json).data
    |> Decodable.resourceToRecord;
  let decodeMany = (json: Js.Json.t) : Document.decodedMany(t) => {
    let document = Document.decodeMany(resourceDecoder, json);
    {
      meta: document.meta,
      resources: document.data |> List.map(_, Decodable.resourceToRecord),
    };
  };
};
