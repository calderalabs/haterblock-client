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
        {. "code": string} => unit
      ) =>
      unit
  }
};

[@bs.val] external gapi : gapi = "gapi";

let load = (~libs: string, ~callback: unit => unit) =>
  gapi##load(libs, callback);

let authorize =
    (
      ~clientId: string,
      ~scope: string,
      ~responseType: string,
      ~callback: string => unit
    ) =>
  gapi##auth2##authorize(
    {"client_id": clientId, "scope": scope, "response_type": responseType},
    response =>
    callback(response##code)
  );