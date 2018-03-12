[%bs.raw {|require('./App.css')|}];

type state = {
  loadingMessage: option(string),
  currentUser: option(User.t)
};

type action =
  | Login
  | UserLoaded(User.t)
  | Loading(string)
  | Loaded;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let fetchCurrentUser = callback =>
    Api.request(
      ~method=Fetch.Get,
      ~path="/users/me",
      ~callback=json => callback(json |> User.decode),
      ()
    );
  let login = ({ReasonReact.send}) => {
    send(Loading("Logging in..."));
    Session.login(() => fetchCurrentUser(user => {
      send(UserLoaded(user));
      send(Loaded);
    }));
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
      | Loading(message) => ReasonReact.Update({...state, loadingMessage: Some(message)})
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
          | (None, Some(user)) =>
            ReasonReact.stringToElement(string_of_int(user.id))
          }
        )
      </div>
  };
};
