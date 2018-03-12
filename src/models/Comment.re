type t = {body: string};

let decode = (json: Js.Json.t) =>
  Json.Decode.{body: json |> field("body", string)};

let fetchAll = (callback: array(t) => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=json => callback(json |> JsonApi.decodeArray(decode)),
    ()
  );
