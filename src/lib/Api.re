open Belt;

exception RequestFailed;

let baseHost =
  switch (Global.environment) {
  | "development" => "localhost:4000"
  | "production" => "api.gethaterblock.com"
  | _ => "localhost:4000"
  };

let baseUrl =
  switch (Global.environment) {
  | "development" => {j|http://$baseHost|j}
  | "production" => {j|https://$baseHost|j}
  | _ => {j|http://$baseHost|j}
  };

let requestHeaders = () => {
  let headers = [
    ("Accept", "application/json"),
    ("Content-Type", "application/json"),
    ("X-Requested-With", "XMLHttpRequest"),
  ];
  let token = Dom.Storage.(getItem("token", localStorage));
  (
    switch (token) {
    | Some(token) => [("Authorization", {j|Bearer $(token)|j}), ...headers]
    | None => headers
    }
  )
  |> List.toArray;
};

let request =
    (
      ~method: Fetch.requestMethod,
      ~path: string,
      ~callback: option(Callback.t(Js.Json.t, string))=?,
      ~body: option(list((string, Js.Json.t)))=?,
      ~query: option(list((string, string)))=?,
      (),
    ) => {
  let initBody =
    switch (body) {
    | Some(body) =>
      Some(Fetch.BodyInit.make(Json.stringify(Json.Encode.object_(body))))
    | None => None
    };
  let fullPath =
    switch (query) {
    | Some(query) =>
      let queryString =
        query
        |> List.map(_, ((key, value)) => {j|$(key)=$(value)|j})
        |> List.reduce(_, "", (acc, param) => {j|$(acc)&$(param)|j});
      {j|$(path)?$(queryString)|j};
    | None => path
    };
  let showError = () =>
    switch (AppAlert.instance^) {
    | None => ()
    | Some(instance) =>
      instance.ReasonReact.send(
        SetMessage("Oops, something went wrong, please try that again."),
      )
    };
  Js.Promise.(
    Fetch.fetchWithInit(
      {j|$(baseUrl)$(fullPath)|j},
      Fetch.RequestInit.make(
        ~method_=method,
        ~headers=Fetch.HeadersInit.makeWithArray(requestHeaders()),
        ~body=?initBody,
        (),
      ),
    )
    |> then_(response =>
         if (Fetch.Response.ok(response)) {
           resolve(response);
         } else {
           if (Fetch.Response.status(response) == 401) {
             Global.clearLocalStorage();
             Global.reload();
           } else {
             showError();
           };
           switch (callback) {
           | Some(callback) =>
             callback(Error(Fetch.Response.statusText(response)))
           | None => ()
           };
           reject(RequestFailed);
         }
       )
    |> then_(Fetch.Response.json)
    |> then_(json => {
         switch (callback) {
         | Some(callback) => callback(Success(json))
         | None => ()
         };
         resolve();
       })
    |> catch(_error => {
         switch (callback) {
         | Some(callback) => callback(Error(""))
         | None => ()
         };
         showError();
         resolve();
       })
  )
  |> ignore;
};
