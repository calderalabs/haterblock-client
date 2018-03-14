type t = {id: int};

module UserDecoder =
  JsonApi.MakeDecoder(
    {
      type model = t;
      type attributes = {.};
      let attributesDecoder = (_json: Js.Json.t) : attributes => {};
      let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : model => {
        id: resource.id
      };
    }
  );

let fetch = (callback: t => unit) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=json => callback(json |> UserDecoder.decodeOne),
    ()
  );
