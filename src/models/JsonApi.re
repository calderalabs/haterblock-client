open Json.Decode;

module Resource = {
  type t('a) = {
    id: int,
    attributes: option('a)
  };
};

module Document = {
  type one('a) = {data: Resource.t('a)};
  type many('a) = {data: array(Resource.t('a))};
  let decodeOne =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : one('a) => {
    data: json |> field("data", resourceDecoder)
  };
  let decodeMany =
      (resourceDecoder: Js.Json.t => Resource.t('a), json: Js.Json.t)
      : many('a) => {
    data: json |> field("data", array(resourceDecoder))
  };
};

module type Decodable = {
  type model;
  type attributes;
  let attributesDecoder: Js.Json.t => attributes;
  let resourceToRecord: Resource.t(attributes) => model;
};

module type Decoder = {
  type t;
  let decodeOne: Js.Json.t => t;
  let decodeMany: Js.Json.t => array(t);
};

module MakeDecoder =
       (Decodable: Decodable)
       : (Decoder with type t := Decodable.model) => {
  type t = Decodable.model;
  let resourceDecoder = (json: Js.Json.t) : Resource.t(Decodable.attributes) => {
    id: json |> field("id", int),
    attributes:
      json |> optional(field("attributes", Decodable.attributesDecoder))
  };
  let decodeOne = (json: Js.Json.t) : t =>
    Document.decodeOne(resourceDecoder, json).data
    |> Decodable.resourceToRecord;
  let decodeMany = (json: Js.Json.t) : array(t) =>
    Document.decodeMany(resourceDecoder, json).data
    |> Array.map(Decodable.resourceToRecord);
};
