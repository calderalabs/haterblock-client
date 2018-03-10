type t = {data: Js.Dict.t(string)};

let decode = (json: Js.Json.t) : t =>
  Json.Decode.{data: json |> field("data", dict(string))};
