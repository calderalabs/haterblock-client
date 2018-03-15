[%bs.raw {|require('./App.css')|}];

type state = {
  loadingMessage: option(string),
  currentUser: option(UserData.t)
};

type action =
  | Login
  | UserLoaded(UserData.t)
  | Loading(string)
  | Loaded;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let fetchCurrentUser = callback => UserData.fetch(callback);
  let login = ({ReasonReact.send}) => {
    send(Loading("Logging in..."));
    Session.login(response =>
      switch response {
      | Success =>
        fetchCurrentUser(user => {
          send(UserLoaded(user));
          send(Loaded);
        })
      | Error => send(Loaded)
      }
    );
  };
  {
    ...component,
    initialState: () => {loadingMessage: Some("Loading..."), currentUser: None},
    didMount: ({send}) => {
      fetchCurrentUser(user => {
        send(UserLoaded(user));
        send(Loaded);
      });
      Gapi.load(~libs="auth2:client", ~callback=() => send(Loaded));
      ReasonReact.NoUpdate;
    },
    reducer: (action, state) =>
      switch action {
      | Loading(message) =>
        ReasonReact.Update({...state, loadingMessage: Some(message)})
      | Loaded => ReasonReact.Update({...state, loadingMessage: None})
      | Login => ReasonReact.SideEffects((self => login(self)))
      | UserLoaded(user) =>
        ReasonReact.Update({...state, currentUser: Some(user)})
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
          | (None, Some(user)) => <Dashboard user />
          }
        )
      </div>
  };
};
