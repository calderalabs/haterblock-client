type t = {
  id: int,
  body: string
};

type attributes = {body: string};

let decode = (json: Js.Json.t) =>
  Json.Decode.{body: json |> field("body", string)};

let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) => {
  id: resource.id,
  body: resource.attributes.body
};

let fetchAll = (callback: array(t) => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/comments",
    ~callback=
      json =>
        callback(
          json |> JsonApi.Document.decodeMany(decode, resourceToRecord)
        ),
    ()
  );
