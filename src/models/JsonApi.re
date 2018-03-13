open Json.Decode;

module Resource = {
  type t('a) = {
    id: int,
    attributes: 'a
  };

  let decode = (attributesDecoder: Json.Decode.decoder('a), json: Js.Json.t) => {
    id: json |> field("id", int),
    attributes: json |> field("attributes", attributesDecoder)
  };
};

module Document = {
  type one('a) = {
    data: Resource.t('a)
  };

  type many('a) = {
    data: array(Resource.t('a))
  };

  let decodeOne =
      (attributesDecoder: Json.Decode.decoder('a), resourceToRecord: (Resource.t('a) => 'b), json: Js.Json.t) => {
    let document: one('a) = {
      data: json |> field("data", Resource.decode(attributesDecoder))
    };

    resourceToRecord(document.data);
  };

  let decodeMany =
      (attributesDecoder: Json.Decode.decoder('a), resourceToRecord: (Resource.t('a) => 'b), json: Js.Json.t) => {
      let document: many('a) = {
        data: json |> field("data", array(Resource.decode(attributesDecoder)))
      };

      document.data |> Array.map(resourceToRecord);
    }
};
