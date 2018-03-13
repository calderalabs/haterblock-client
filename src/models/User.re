type t = {id: int};

let attributesDecoder = (_json: Js.Json.t) => None;

let resourceToRecord = (resource: JsonApi.Resource.t('a)) => {
  id: resource.id
};

let fetch = (callback: t => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      json =>
        callback(json |> JsonApi.Document.decodeOne(attributesDecoder, resourceToRecord)),
    ()
  );
