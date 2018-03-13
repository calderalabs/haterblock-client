type t = {id: int};

let resourceToRecord = (resource: JsonApi.Resource.t('a)) => {id: resource.id};

let fetch = (callback: t => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      json => callback(json |> JsonApi.Document.decodeOne(resourceToRecord)),
    ()
  );
