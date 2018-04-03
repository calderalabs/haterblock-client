open Belt;

module User = {
  type t = {
    id: Model.id,
    name: string,
    email: string,
    syncing: bool,
  };
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = {
          name: string,
          email: string,
          syncing: bool,
        };
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{
            name: json |> field("name", string),
            email: json |> field("email", string),
            syncing: json |> field("syncing", bool),
          };
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {id: resource.id, name: "", email: "", syncing: false}
          | Some(attributes) => {
              id: resource.id,
              name: attributes.name,
              email: attributes.email,
              syncing: attributes.syncing,
            }
          };
      },
    );
};

let fetch = (callback: Callback.t(User.t, string)) =>
  Api.request(
    ~method=Fetch.Get,
    ~path="/users/me",
    ~callback=
      response =>
        switch (response) {
        | Success(json) => callback(Success(json |> User.decodeOne))
        | Error(error) => callback(Error(error))
        },
    (),
  );
