open Belt;

[%bs.raw {|require('./Dashboard.css')|}];

type state = {syncing: bool};

type action =
  | StartedSyncing
  | FinishedSyncing;

let component = ReasonReact.reducerComponent("Dashboard");

let make = (~user: UserData.User.t, _children) => {
  ...component,
  initialState: () => {syncing: false},
  reducer: (action, _state) =>
    switch (action) {
    | StartedSyncing => ReasonReact.Update({syncing: true})
    | FinishedSyncing => ReasonReact.Update({syncing: false})
    },
  didMount: _self =>
    switch (Session.getToken()) {
    | None => ReasonReact.NoUpdate
    | Some(token) =>
      ReasonReact.SideEffects(
        (
          ({ReasonReact.send}) =>
            UserChannel.join(~user, ~token, ~callback=payload => {
              if (Js.Boolean.to_js_boolean(payload##syncing) == Js.true_) {
                send(StartedSyncing);
              } else {
                send(FinishedSyncing);
              };
            }
            )
            |> ignore
        ),
      )
    },
  render: ({ReasonReact.state}) =>
    <div className="Dashboard">
      (
        if (state.syncing) {
          ReasonReact.stringToElement("Syncing");
        } else {
          ReasonReact.nullElement;
        }
      )
      <CommentList sentiment=Hateful />
      <CommentList sentiment=Negative />
      <CommentList sentiment=Neutral />
      <CommentList sentiment=Positive />
    </div>,
};
