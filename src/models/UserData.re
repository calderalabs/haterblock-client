open Belt;

open MomentRe;

module User = {
  type t = {
    id: Model.id,
    name: string,
    email: string,
    syncedAt: option(Moment.t),
  };
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = {
          name: string,
          email: string,
          syncedAt: Js.Null.t(string),
        };
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{
            name: json |> field("name", string),
            email: json |> field("email", string),
            syncedAt: json |> nullable(field("synced_at", string)),
          };
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {id: resource.id, name: "", email: "", syncedAt: None}
          | Some(attributes) =>
            let syncedAt =
              switch (Js.Null.toOption(attributes.syncedAt)) {
              | None => None
              | Some(syncedAt) => Some(moment(syncedAt))
              };
            {
              id: resource.id,
              name: attributes.name,
              email: attributes.email,
              syncedAt,
            };
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
