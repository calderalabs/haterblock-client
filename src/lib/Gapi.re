open Belt;

let auth2 = ref(None);

type response = {
  .
  "code": string,
  [@bs.return nullable] "error": option(string),
};

type gapi = {
  .
  [@bs.meth] "load": (string, unit => unit) => unit,
  "auth2": {
    .
    [@bs.meth]
    "init":
      {
        .
        "client_id": string,
        "scope": string,
      } =>
      {
        .
        [@bs.meth]
        "grantOfflineAccess":
          unit => {. [@bs.meth] "_then": (response => unit) => unit},
      },
  },
};

[@bs.val] external gapi : gapi = "gapi";

let load = (~libs: string, ~callback: unit => unit) =>
  gapi##load(libs, callback);

let init = (~clientId: string, ~scope: string) =>
  auth2 := Some(gapi##auth2##init({"client_id": clientId, "scope": scope}));

let grantOfflineAccess = (callback: response => unit) =>
  switch (auth2^) {
  | Some(auth2) =>
    auth2##grantOfflineAccess()##_then(response => callback(response))
  | None => ()
  };
