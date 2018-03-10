type t = {token: string};

let decode = json : t => Json.Decode.{token: json |> field("token", string)};

let login = (callback: unit => unit) =>
  Gapi.authorize(
    ~clientId=
      "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
    ~scope="profile email https://www.googleapis.com/auth/youtube.force-ssl",
    ~responseType="code",
    ~callback=code =>
    Api.request(
      ~method=Fetch.Post,
      ~path="/auth/google/callback",
      ~body=[("code", Json.Encode.string(code))],
      ~callback=
        json =>
          json
          |> decode
          |> (
            session => {
              Dom.Storage.setItem(
                "token",
                session.token,
                Dom.Storage.localStorage
              );
              callback();
            }
          ),
      ()
    )
  );
