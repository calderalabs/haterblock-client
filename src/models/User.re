type t = {id: string};

let decode = json : t => {
  let jsonApi = JsonApi.decode(json);
  Json.Decode.{id: Js.Json.object_(jsonApi.data) |> field("id", string)};
};
