open Belt;

[%bs.raw {|require('./App.css')|}];

type page =
  | Dashboard
  | Settings;

type state = {
  loadingMessage: option(string),
  currentUser: option(UserData.User.t),
  currentPage: page,
};

type action =
  | Login
  | UserLoaded(UserData.User.t, unit => unit)
  | Loading(string)
  | Loaded(option(UserData.User.t))
  | Logout
  | ShowPage(page);

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let fetchCurrentUser = (~callback=() => (), send) =>
    switch (Session.getToken()) {
    | None => send(Loaded(None))
    | Some(_) =>
      UserData.fetch(response => {
        switch (response) {
        | Success(user) => {
          send(UserLoaded(user, callback));
          send(Loaded(Some(user)));
        }
        | Error(_) => send(Loaded(None))
        };
      })
    };
  let login = ({ReasonReact.send}) => {
    send(Loading("Logging in..."));
    Session.login(response =>
      switch (response) {
      | Success () =>
        fetchCurrentUser(
          ~callback=() => ReasonReact.Router.push("/dashboard"),
          send,
        )
      | Error(_error) => send(Loaded(None))
      }
    );
  };
  let router = (self, url) =>
    switch (url.ReasonReact.Router.path) {
    | ["dashboard"] => self.ReasonReact.send(ShowPage(Dashboard))
    | ["settings"] => self.send(ShowPage(Settings))
    | _ => self.send(ShowPage(Dashboard))
    };
  {
    ...component,
    initialState: () => {
      loadingMessage: Some("Loading..."),
      currentUser: None,
      currentPage: Dashboard,
    },
    didMount: ({ReasonReact.send}) => {
      let initialUrl = ReasonReact.Router.dangerouslyGetInitialUrl();
      Gapi.load(~libs="auth2:client", ~callback=() => {
        Gapi.init(
          ~clientId=
            "1008243769527-gv6uq08kuatnu5gcdvd5bggtkpc5mc0k.apps.googleusercontent.com",
          ~scope=
            "profile email https://www.googleapis.com/auth/youtube.force-ssl",
        );
        fetchCurrentUser(
          ~callback=
            () =>
              switch (initialUrl.path) {
              | [] => ReasonReact.Router.push("/dashboard")
              | _ => ()
              },
          send,
        );
      });
      ReasonReact.SideEffects(self => router(self, initialUrl));
    },
    reducer: (action, state) =>
      switch (action) {
      | Loading(message) =>
        ReasonReact.Update({...state, loadingMessage: Some(message)})
      | Loaded(user) => switch user {
        | None => ReasonReact.UpdateWithSideEffects({...state, loadingMessage: None}, (_) => ReasonReact.Router.push("/"))
        | Some(user) =>  ReasonReact.Update({...state, loadingMessage: None})
      }
      | Login => ReasonReact.SideEffects((self => login(self)))
      | UserLoaded(user, callback) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, currentUser: Some(user)},
          ((_) => callback()),
        )
      | Logout =>
        ReasonReact.UpdateWithSideEffects(
          {...state, currentUser: None},
          (
            (_) => {
              ReasonReact.Router.push("/");
              Global.clearSessionToken();
            }
          ),
        )
      | ShowPage(page) => ReasonReact.Update({...state, currentPage: page})
      },
    subscriptions: self => [
      Sub(
        () => ReasonReact.Router.watchUrl(router(self)),
        ReasonReact.Router.unwatchUrl,
      ),
    ],
    render: ({state, send}) => {
      let navClassName = page =>
        page == state.currentPage ?
          "App__navItem App__navItem--active" : "App__navItem";
      <div className="App">
        <div className="App__header">
          <div className="App__title">
            (ReasonReact.stringToElement("HaterBlock"))
          </div>
          <div className="App__headerItems">
            (
              switch (state.currentUser) {
              | Some(user) =>
                [|
                  <div key="0" className="App__currentUserName">
                    (ReasonReact.stringToElement(user.name))
                  </div>,
                  <button
                    key="1"
                    className="Button Button--small"
                    onClick=(_event => send(Logout))>
                    (ReasonReact.stringToElement("Logout"))
                  </button>,
                |]
                |> ReasonReact.arrayToElement
              | None =>
                <button
                  onClick=(_event => send(Login))
                  className="Button Button--small">
                  (ReasonReact.stringToElement("Login"))
                </button>
              }
            )
          </div>
        </div>
        (
          switch (state.currentUser) {
          | None => ReasonReact.nullElement
          | Some(_) =>
            <div className="App__nav">
              <div className="App__navInner">
                <div className="App__navContainer">
                  <a
                    className=(navClassName(Dashboard))
                    onClick=((_) => ReasonReact.Router.push("/dashboard"))>
                    (ReasonReact.stringToElement("Dashboard"))
                  </a>
                  <a
                    className=(navClassName(Settings))
                    onClick=((_) => ReasonReact.Router.push("/settings"))>
                    (ReasonReact.stringToElement("Settings"))
                  </a>
                </div>
              </div>
            </div>
          }
        )
        <div className="App__content">
          <div className="App__contentInner">
            <AppAlert />
            (
              switch (state.currentUser, state.loadingMessage) {
              | (_, Some(loadingMessage)) =>
                <MessageBox>
                  (ReasonReact.stringToElement(loadingMessage))
                </MessageBox>
              | (Some(user), None) =>
                switch (state.currentPage) {
                | Dashboard =>
                  <Dashboard
                    user
                    onUserChannelJoined=(() => fetchCurrentUser(send))
                  />
                | Settings =>
                  <Settings
                    user
                    onUserUpdated=(user => send(UserLoaded(user, () => ())))
                  />
                }
              | (None, _) => <Landing onLogin=(() => send(Login)) />
              }
            )
          </div>
        </div>
      </div>;
    },
  };
};
