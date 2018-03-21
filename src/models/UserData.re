open Belt;

module User = {
  type t = {id: Model.id};
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = option({.});
        let attributesDecoder = (_json: Js.Json.t) : attributes => None;
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t => {id: resource.id};
      }
    );
};

let fetch = (callback: Callback.t(User.t, string)) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      (response) =>
        switch response {
        | Success(json) => callback(Success(json |> User.decodeOne))
        | Error(error) => callback(Error(error))
        },
    ()
  );
