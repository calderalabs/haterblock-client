open Belt;

type t = {token: string};

let decode = (json: Js.Json.t) =>
  Json.Decode.{token: json |> field("token", string)};

let persistToken = (callback: Callback.t(unit, unit), json: Js.Json.t) =>
  json
  |> decode
  |> (
    session => {
      Dom.Storage.(setItem("token", session.token, localStorage));
      callback(Success());
    }
  );

let getToken = () => Dom.Storage.(getItem("token", localStorage));

let login = (callback: Callback.t(unit, unit)) =>
  Gapi.grantOfflineAccess(response =>
    switch (response##error) {
    | Some(_error) => callback(Error())
    | None =>
      Api.request(
        ~method=Fetch.Post,
        ~path="/auth/google/callback",
        ~body=[("code", Json.Encode.string(response##code))],
        ~callback=
          response =>
            switch (response) {
            | Success(json) => json |> persistToken(callback)
            | Error(_error) => callback(Error())
            },
        (),
      )
    }
  );

let logout = () =>
  Dom.Storage.(removeItem("token", localStorage));
