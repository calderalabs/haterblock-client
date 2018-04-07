open Belt;

open MomentRe;

[%bs.raw {|require('./Dashboard.css')|}];

type state = {
  syncedAt: option(Moment.t),
  newCommentCount: int,
};

type action =
  | FinishedSyncing(option(Moment.t), int);

let component = ReasonReact.reducerComponent("Dashboard");

let make = (~user: UserData.User.t, _children) => {
  ...component,
  initialState: () => {syncedAt: user.syncedAt, newCommentCount: 0},
  reducer: (action, state) =>
    switch (action) {
    | FinishedSyncing(syncedAt, newCommentCount) =>
      ReasonReact.Update({
        syncedAt,
        newCommentCount:
          user.syncedAt == None ? 0 : state.newCommentCount + newCommentCount,
      })
    },
  didMount: _self =>
    switch (Session.getToken()) {
    | None => ReasonReact.NoUpdate
    | Some(token) =>
      ReasonReact.SideEffects(
        (
          ({ReasonReact.send}) =>
            UserChannel.join(
              ~user,
              ~token,
              ~callback=payload => {
                let syncedAt =
                  switch (Js.Null.toOption(payload##synced_at)) {
                  | None => None
                  | Some(syncedAt) => Some(moment(syncedAt))
                  };
                send(FinishedSyncing(syncedAt, payload##new_comment_count));
              },
            )
            |> ignore
        ),
      )
    },
  render: ({ReasonReact.state}) =>
    <div className="Dashboard">
      (
        if (state.syncedAt == None) {
          <MessageBox>
            (ReasonReact.stringToElement("Syncing..."))
          </MessageBox>;
        } else {
          <div>
            (
              if (state.newCommentCount != 0) {
                let count = state.newCommentCount;
                let message =
                  state.newCommentCount == 1 ?
                    "There is 1 new comment, refresh the page to see it." :
                    {j|There are $count new comments, refresh the page to see them.|j};
                <div className="App__loadingMessageWrapper">
                  <div className="App__loadingMessage">
                    (ReasonReact.stringToElement(message))
                  </div>
                </div>;
              } else {
                ReasonReact.nullElement;
              }
            )
            <CommentList sentiment=Negative />
            <CommentList sentiment=Neutral />
            <CommentList sentiment=Positive />
          </div>;
        }
      )
    </div>,
};
