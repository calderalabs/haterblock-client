type t = {id: int};

type attributes = {.};

let decode = (_json: Js.Json.t) => {};

let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) => {
  id: resource.id
};

let fetch = (callback: t => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      json =>
        callback(json |> JsonApi.Document.decodeOne(decode, resourceToRecord)),
    ()
  );
