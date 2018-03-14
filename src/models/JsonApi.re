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
};

module type AttributesDecoder = {
  type model;
  type attributes;
  let attributesDecoder: Js.Json.t => attributes;
  let resourceToRecord: Resource.t(attributes) => model;
};

module type Decoder = {
  type model;
  let decodeOne: Js.Json.t => model;
  let decodeMany: Js.Json.t => array(model);
};

module MakeDecoder =
       (Decoder: AttributesDecoder)
       : (Decoder with type model := Decoder.model) => {
  type model = Decoder.model;
  type attributes = Decoder.attributes;
  let attributesDecoder = Decoder.attributesDecoder;
  let resourceToRecord = Decoder.resourceToRecord;
  let resourceDecoder = (json: Js.Json.t) : Resource.t(attributes) => {
    id: json |> field("id", int),
    attributes: json |> optional(field("attributes", attributesDecoder))
  };
  let decodeOne = (json: Js.Json.t) : model => {
    let document: Document.one(attributes) = {
      data: json |> field("data", resourceDecoder)
    };
    resourceToRecord(document.data);
  };
  let decodeMany = (json: Js.Json.t) : array(model) => {
    let document: Document.many(attributes) = {
      data: json |> field("data", array(resourceDecoder))
    };
    document.data |> Array.map(resourceToRecord);
  };
};
