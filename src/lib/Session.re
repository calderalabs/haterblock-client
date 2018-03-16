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

let login = (callback: Callback.t(unit, unit)) =>
  Gapi.authorize(
    ~clientId=
      "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
    ~scope="profile email https://www.googleapis.com/auth/youtube.force-ssl",
    ~responseType="code",
    ~callback=response =>
    switch response##error {
    | Some(_error) => callback(Error())
    | None =>
      Api.request(
        ~method=Fetch.Post,
        ~path="/auth/google/callback",
        ~body=[("code", Json.Encode.string(response##code))],
        ~callback=
          response =>
            switch response {
            | Success(json) => json |> persistToken(callback)
            | Error(_error) => callback(Error())
            },
        ()
      )
    }
  );
