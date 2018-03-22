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
  type one('a) = {data: Resource.t('a)};
  type many('a) = {data: list(Resource.t('a))};
  let decodeOne =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : one('a) => {
    data: json |> field("data", resourceDecoder),
  };
  let decodeMany =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : many('a) => {
    data: json |> field("data", list(resourceDecoder)),
  };
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
  let decodeMany: Js.Json.t => list(t);
};

module MakeDecoder =
       (Decodable: Decodable)
       : (Decoder with type t := Decodable.t) => {
  type t = Decodable.t;
  let resourceDecoder = Resource.decode(Decodable.attributesDecoder);
  let decodeOne = (json: Js.Json.t) : t =>
    Document.decodeOne(resourceDecoder, json).data
    |> Decodable.resourceToRecord;
  let decodeMany = (json: Js.Json.t) : list(t) =>
    Document.decodeMany(resourceDecoder, json).data
    |> List.map(_, Decodable.resourceToRecord);
};
