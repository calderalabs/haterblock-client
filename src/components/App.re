[%bs.raw {|require('./App.css')|}];

type state = {
  loading: bool,
  currentUser: option(User.t)
};

type action =
  | Login
  | UserLoaded(User.t)
  | Loading
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
    send(Loading);
    Session.login(() => fetchCurrentUser(user =>
      send(UserLoaded(user))
    ));
  };

  {
    ...component,
    initialState: () => {loading: true, currentUser: None},
    didMount: ({send}) => {
      fetchCurrentUser(user => send(UserLoaded(user)));
      Gapi.load(~libs="auth2:client", ~callback=() => send(Loaded));
      ReasonReact.NoUpdate;
    },
    reducer: (action, state) =>
      switch action {
      | Loading => ReasonReact.Update({...state, loading: true})
      | Loaded => ReasonReact.Update({...state, loading: false})
      | Login => ReasonReact.SideEffects(self => login(self))
      | UserLoaded(user) => ReasonReact.Update({loading: false, currentUser: Some(user)})
      },
    render: ({state, send}) =>
      <div className="App">
        (
          switch (state.loading, state.currentUser) {
          | (true, _) => ReasonReact.nullElement
          | (false, None) =>
            <button onClick=((_event) => send(Login))>
              (ReasonReact.stringToElement("Login"))
            </button>
          | (false, Some(user)) =>
            ReasonReact.stringToElement(string_of_int(user.id))
          }
        )
      </div>
  };
};
