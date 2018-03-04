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

[@bs.val] external environment : string = "process.env.NODE_ENV";

[@bs.val] external gapi : gapi = "gapi";

[@bs.scope "JSON"] [@bs.val]
external jsonStringify : Js.t('a) => string = "stringify";

let apiBaseUrl =
  switch environment {
  | "development" => "http://localhost:4000"
  | _ => "http://localhost:4000"
  };

let callbackUrl = {j|$apiBaseUrl/auth/google/callback|j};

let login = () =>
  gapi##auth2##authorize(
    {
      "client_id": "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
      "scope": "profile email https://www.googleapis.com/auth/youtube.force-ssl",
      "response_type": "code"
    },
    response =>
    Js.Promise.(
      Fetch.fetchWithInit(
        callbackUrl,
        Fetch.RequestInit.make(
          ~method_=Post,
          ~headers=
            Fetch.HeadersInit.make({
              "Accept": "application/json",
              "Content-Type": "application/json",
              "X-Requested-With": "XMLHttpRequest"
            }),
          ~body=Fetch.BodyInit.make(jsonStringify({"code": response##code})),
          ()
        )
      )
      |> resolve
    )
    |> ignore
  );

let load = callback => gapi##load("auth2", callback);
