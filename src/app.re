[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

type state = {loading: bool};

type action =
  | Loaded;

let component = ReasonReact.reducerComponent("App");

let make = _children => {
  let login = _event => Session.login();
  {
    ...component,
    initialState: () => {loading: true},
    didMount: ({ReasonReact.reduce}) => {
      Api.request(~method=Fetch.Get, ~path="/users/me", ~callback=(json) => json |> Js.log, ());
      Session.load(() => reduce(() => Loaded, ()));
      ReasonReact.NoUpdate;
    },
    reducer: (action, _state) =>
      switch action {
      | Loaded => ReasonReact.Update({loading: false})
      },
    render: self =>
      <div className="App">
        (
          if (self.state.loading) {
            ReasonReact.nullElement;
          } else {
            <button onClick=login>
              (ReasonReact.stringToElement("Login"))
            </button>;
          }
        )
      </div>
  };
};
