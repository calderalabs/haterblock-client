let decode = (decoder: Json.Decode.decoder('a), json: Js.Json.t) =>
  Json.Decode.(json |> field("data", decoder));
