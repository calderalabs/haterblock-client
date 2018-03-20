open Belt;

module User = {
  type t = {id: int};
};

include
  JsonApi.MakeDecoder(
    {
      type model = User.t;
      type attributes = option({.});
      let attributesDecoder = (_json: Js.Json.t) : attributes => None;
      let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : model => {
        id: resource.id
      };
    }
  );

let fetch = (callback: Callback.t(User.t, string)) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      response =>
        switch response {
        | Success(json) => callback(Success(json |> decodeOne))
        | Error(error) => callback(Error(error))
        },
    ()
  );
