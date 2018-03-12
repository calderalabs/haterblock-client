let decodeDocument = (decoder: Json.Decode.decoder('a), json: Js.Json.t) =>
  Json.Decode.(json |> field("data", decoder));

let decodeOne = (decoder: Json.Decode.decoder('a), json: Js.Json.t) : 'a =>
  json |> decodeDocument(json => decoder(json));

let decodeArray =
    (decoder: Json.Decode.decoder('a), json: Js.Json.t)
    : array('a) =>
  json |> decodeDocument(json => Json.Decode.(json |> array(decoder)));
