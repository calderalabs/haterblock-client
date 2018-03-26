open Belt;

[%bs.raw {|require('./Dashboard.css')|}];

type state = {comments: option(list(CommentData.Comment.t))};

type action =
  | CommentsLoaded(list(CommentData.Comment.t));

let component = ReasonReact.reducerComponent("Dashboard");

let make = (~user: UserData.User.t, _children) => {
  let loadComments = ({ReasonReact.send}) =>
    CommentData.fetchAll(response =>
      switch (response) {
      | Success(comments) => send(CommentsLoaded(comments))
      | Error () => ()
      }
    );
  {
    ...component,
    initialState: () => {comments: None},
    reducer: (action, _state: state) =>
      switch (action) {
      | CommentsLoaded(comments) =>
        ReasonReact.Update({comments: Some(comments)})
      },
    didMount: self => {
      loadComments(self);
      ReasonReact.NoUpdate;
    },
    render: ({state}) =>
      <div className="Dashboard">
        (
          switch (state.comments) {
          | None =>
            ReasonReact.stringToElement("There are no comments to display")
          | Some(comments) =>
            let filterBySentiment =
              CommentData.Sentiment.filterBySentiment(comments);
            <div>
              <CommentList comments=(filterBySentiment(Hateful)) />
              <CommentList comments=(filterBySentiment(Negative)) />
              <CommentList comments=(filterBySentiment(Neutral)) />
              <CommentList comments=(filterBySentiment(Positive)) />
            </div>;
          }
        )
      </div>,
  };
};
