[%bs.raw {|require('./App.css')|}];

type state = {
  loading: bool,
  currentUser: option(User.t)
};

type action =
  | Login(User.t)
  | Loading
  | Loaded;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let fetchCurrentUser = send =>
    Api.request(
      ~method=Fetch.Get,
      ~path="/users/me",
      ~callback=json => send(Login(json |> User.decode)),
      ()
    );
  let login = (_event, {ReasonReact.send}) => {
    send(Loading);
    Session.login(() => fetchCurrentUser(send));
  };
  {
    ...component,
    initialState: () => {loading: true, currentUser: None},
    didMount: ({ReasonReact.send}) => {
      fetchCurrentUser(send);
      Gapi.load(~libs="auth2:client", ~callback=() => send(Loaded));
      ReasonReact.NoUpdate;
    },
    reducer: (action, state) =>
      switch action {
      | Loading => ReasonReact.Update({...state, loading: true})
      | Loaded => ReasonReact.Update({...state, loading: false})
      | Login(user) =>
        ReasonReact.Update({loading: false, currentUser: Some(user)})
      },
    render: ({state, handle}) =>
      <div className="App">
        (
          switch (state.loading, state.currentUser) {
          | (true, _) => ReasonReact.nullElement
          | (false, None) =>
            <button onClick=(handle(login))>
              (ReasonReact.stringToElement("Login"))
            </button>
          | (false, Some(user)) =>
            ReasonReact.stringToElement(string_of_int(user.id))
          }
        )
      </div>
  };
};
