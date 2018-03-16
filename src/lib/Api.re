[@bs.val] external environment : string = "process.env.NODE_ENV";

let baseUrl =
  switch environment {
  | "development" => "http://localhost:4000"
  | _ => "http://localhost:4000"
  };

let _requestHeaders = () => {
  let headers = [
    ("Accept", "application/json"),
    ("Content-Type", "application/json"),
    ("X-Requested-With", "XMLHttpRequest")
  ];
  let token = Dom.Storage.getItem("token", Dom.Storage.localStorage);
  (
    switch token {
    | Some(token) => [("Authorization", {j|Bearer $(token)|j}), ...headers]
    | None => headers
    }
  )
  |> Array.of_list;
};

let request =
    (
      ~method: Fetch.requestMethod,
      ~path: string,
      ~callback: option(Callback.t(Js.Json.t, unit))=?,
      ~body: option(list((string, Js.Json.t)))=?,
      ()
    ) => {
  let initBody =
    switch body {
    | Some(body) =>
      Some(Fetch.BodyInit.make(Json.stringify(Json.Encode.object_(body))))
    | None => None
    };
  Js.Promise.(
    Fetch.fetchWithInit(
      {j|$(baseUrl)$(path)|j},
      Fetch.RequestInit.make(
        ~method_=method,
        ~headers=Fetch.HeadersInit.makeWithArray(_requestHeaders()),
        ~body=?initBody,
        ()
      )
    )
    |> then_(Fetch.Response.json)
    |> then_(json => {
         switch callback {
         | Some(callback) => callback(Success(json))
         | None => ()
         };
         resolve();
       })
  )
  |> ignore;
};
