type t = {id: int};

let decode = (json: Js.Json.t) : t =>
  Json.Decode.{id: json |> field("id", int)};
