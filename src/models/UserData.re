open Belt;

open MomentRe;

module User = {
  type t = {
    id: Model.id,
    name: string,
    email: string,
    syncedAt: option(Moment.t),
    autoRejectEnabled: bool,
    emailNotificationsEnabled: bool,
  };
  include
    JsonApi.MakeDecoder(
      {
        type nonrec t = t;
        type attributes = {
          name: string,
          email: string,
          syncedAt: Js.Null.t(string),
          autoRejectEnabled: bool,
          emailNotificationsEnabled: bool,
        };
        let attributesDecoder = (json: Js.Json.t) : attributes =>
          Json.Decode.{
            name: json |> field("name", string),
            email: json |> field("email", string),
            syncedAt: json |> nullable(field("synced_at", string)),
            autoRejectEnabled: json |> field("auto_reject_enabled", bool),
            emailNotificationsEnabled:
              json |> field("email_notifications_enabled", bool),
          };
        let resourceToRecord = (resource: JsonApi.Resource.t(attributes)) : t =>
          switch (resource.attributes) {
          | None => {
              id: resource.id,
              name: "",
              email: "",
              syncedAt: None,
              autoRejectEnabled: false,
              emailNotificationsEnabled: true,
            }
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
              autoRejectEnabled: attributes.autoRejectEnabled,
              emailNotificationsEnabled: attributes.emailNotificationsEnabled,
            };
          };
      },
    );
};

let fetch = (callback: Callback.t(User.t, Api.error)) =>
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

let update = (user: User.t, callback: Callback.t(unit, Api.error)) =>
  Api.request(
    ~method=Fetch.Put,
    ~path="/users/me",
    ~body=[
      Json.Encode.(
        "user",
        object_([
          (
            "auto_reject_enabled",
            boolean(Js.Boolean.to_js_boolean(user.autoRejectEnabled)),
          ),
          (
            "email_notifications_enabled",
            boolean(Js.Boolean.to_js_boolean(user.emailNotificationsEnabled)),
          )
        ]),
      ),
    ],
    ~callback=
      response =>
        switch (response) {
        | Success(_json) => callback(Success())
        | Error(error) => callback(Error(error))
        },
    (),
  );
