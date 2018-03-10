type t = {id: int};

let decode = (json: Js.Json.t) : t =>
  json |> JsonApi.decode(json => Json.Decode.{id: json |> field("id", int)});
