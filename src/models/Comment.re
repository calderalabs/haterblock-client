type t = {body: string};

let decode = (json: Js.Json.t) : t =>
  json
  |> JsonApi.decode(json => Json.Decode.{body: json |> field("body", string)});
