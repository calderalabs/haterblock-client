open Belt;

[%bs.raw {|require('./App.css')|}];

type state = {
  loadingMessage: option(string),
  currentUser: option(UserData.User.t),
};

type action =
  | Login
  | UserLoaded(UserData.User.t)
  | Loading(string)
  | Loaded
  | Logout;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let fetchCurrentUser = send =>
    switch (Session.getToken()) {
    | None => send(Loaded)
    | Some(_) =>
      UserData.fetch(
        (
          user => {
            send(UserLoaded(user));
            send(Loaded);
          }
        )
        |> Callback.ignoreError,
      )
    };
  let login = ({ReasonReact.send}) => {
    send(Loading("Logging in..."));
    Session.login(response =>
      switch (response) {
      | Success () => fetchCurrentUser(send)
      | Error(_error) => send(Loaded)
      }
    );
  };
  {
    ...component,
    initialState: () => {
      loadingMessage: Some("Loading..."),
      currentUser: None,
    },
    didMount: ({ReasonReact.send}) => {
      Gapi.load(~libs="auth2:client", ~callback=() => {
        Gapi.init(
          ~clientId=
            "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
          ~scope=
            "profile email https://www.googleapis.com/auth/youtube.force-ssl",
        );
        fetchCurrentUser(send);
      });
      ReasonReact.NoUpdate;
    },
    reducer: (action, state) =>
      switch (action) {
      | Loading(message) =>
        ReasonReact.Update({...state, loadingMessage: Some(message)})
      | Loaded => ReasonReact.Update({...state, loadingMessage: None})
      | Login => ReasonReact.SideEffects((self => login(self)))
      | UserLoaded(user) =>
        ReasonReact.Update({...state, currentUser: Some(user)})
      | Logout =>
        ReasonReact.UpdateWithSideEffects(
          {...state, currentUser: None},
          ((_) => Session.logout()),
        )
      },
    render: ({state, send}) =>
      <div className="App">
        <div className="App__header">
          <div className="App__title">
            (ReasonReact.stringToElement("Haterblock"))
          </div>
          <div className="App__headerItems">
            (
              switch (state.currentUser) {
              | Some(user) =>
                <div className="App__currentUserName">
                  <button onClick=(_event => send(Logout))>
                    (ReasonReact.stringToElement("Logout"))
                  </button>
                  <div> (ReasonReact.stringToElement(user.name)) </div>
                </div>
              | None =>
                <button onClick=(_event => send(Login))>
                  (ReasonReact.stringToElement("Login"))
                </button>
              }
            )
          </div>
        </div>
        <div className="App__content">
          (
            switch (state.loadingMessage) {
            | Some(loadingMessage) =>
              <span> (ReasonReact.stringToElement(loadingMessage)) </span>
            | None => <Dashboard />
            }
          )
        </div>
        <div className="App__footer">
          <a href="http://calderalabs.io" className="App__caldera-labs-logo">
            <img
              src="https://dotamastery.io/assets/images/caldera-labs-logo-6a708f5b7a4915b16e468c86c3039a0c.png"
            />
          </a>
          <div>
            <small>
              (
                ReasonReact.stringToElement(
                  {j|Copyright © 2018 Caldera Labs Ltd.|j},
                )
              )
            </small>
          </div>
        </div>
      </div>,
  };
};
