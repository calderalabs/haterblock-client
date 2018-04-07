open Belt;

type error =
  | Unknown(string)
  | Request(list(JsonApi.Error.t));

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
      ~callback: option(Callback.t(Js.Json.t, error))=?,
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
  let showError = _error =>
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
         response
         |> Fetch.Response.json
         |> then_(json => {
              if (response |> Fetch.Response.ok) {
                switch (callback) {
                | Some(callback) => callback(Success(json))
                | None => ()
                };
              } else {
                let errors = json |> JsonApi.Error.decodeMany;
                let error = Request(errors);
                if (errors |> List.some(_, error => error.code == "401")) {
                  Global.clearSessionToken();
                  Global.reload();
                } else {
                  error |> showError;
                };
                switch (callback) {
                | Some(callback) => callback(Error(error))
                | None => ()
                };
              };
              resolve();
            })
       )
    |> catch(error => {
         let error = Unknown(Obj.magic(error)##toString());
         switch (callback) {
         | Some(callback) => callback(Error(error))
         | None => ()
         };
         error |> showError;
         resolve();
       })
  )
  |> ignore;
};
