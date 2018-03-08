type gapi = {
  .
  [@bs.meth] "load": (string, unit => unit) => unit,
  "auth2": {
    .
    [@bs.meth]
    "authorize":
      (
        {
          .
          "client_id": string,
          "scope": string,
          "response_type": string
        },
        {
          .
          "code": string,
          "error": string
        } =>
        unit
      ) =>
      unit
  }
};

[@bs.val] external gapi : gapi = "gapi";

type callback = {token: string};

module Decode = {
  let callback = json : callback =>
    Json.Decode.{token: json |> field("token", string)};
};

let login = () =>
  gapi##auth2##authorize(
    {
      "client_id": "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
      "scope": "profile email https://www.googleapis.com/auth/youtube.force-ssl",
      "response_type": "code"
    },
    response =>
    Api.request(
      ~method=Fetch.Post,
      ~path="/auth/google/callback",
      ~body=[("code", Json.Encode.string(response##code))],
      ~callback=
        json =>
          json
          |> Decode.callback
          |> (
            callback =>
              Dom.Storage.setItem(
                "token",
                callback.token,
                Dom.Storage.localStorage
              )
          ),
      ()
    )
  );

let load = callback => gapi##load("auth2", callback);
