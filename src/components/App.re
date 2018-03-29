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
        (
          switch (state.loadingMessage, state.currentUser) {
          | (Some(loadingMessage), _) =>
            <span> (ReasonReact.stringToElement(loadingMessage)) </span>
          | (None, None) =>
            <button onClick=(_event => send(Login))>
              (ReasonReact.stringToElement("Login"))
            </button>
          | (None, Some(user)) =>
            <div>
              <button onClick=(_event => send(Logout))>
                (ReasonReact.stringToElement("Logout"))
              </button>
              <Dashboard user />
            </div>
          }
        )
      </div>,
  };
};
