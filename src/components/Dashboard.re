open Belt;

[%bs.raw {|require('./Dashboard.css')|}];

type state = {comments: option(list(CommentData.Comment.t))};

type action =
  | CommentsLoaded(list(CommentData.Comment.t));

let component = ReasonReact.reducerComponent("Dashboard");

let comments: list(CommentData.Comment.t) = [
  {
    id: 1,
    body: "I don't like this",
    score: (-6),
    videoId: 0,
    rejected: false,
  },
  {
    id: 2,
    body: "This is not so great but ok",
    score: (-3),
    videoId: 0,
    rejected: false,
  },
  {
    id: 3,
    body: "Awful content mate",
    score: (-5),
    videoId: 0,
    rejected: false,
  },
  {
    id: 4,
    body: "This is fucking shit",
    score: (-8),
    videoId: 0,
    rejected: false,
  },
  {
    id: 5,
    body: "You should kill yourself",
    score: (-7),
    videoId: 0,
    rejected: false,
  },
  {
    id: 6,
    body: "This is total garbage",
    score: (-9),
    videoId: 0,
    rejected: false,
  },
  {id: 7, body: "This is really good", score: 5, videoId: 0, rejected: false},
];

let make = (~user: UserData.User.t, _children) => {
  let loadComments = ({ReasonReact.send}) =>
    send(CommentsLoaded(comments));
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
              <CommentList comments=(filterBySentiment(Positive)) />
              <CommentList comments=(filterBySentiment(Neutral)) />
              <CommentList comments=(filterBySentiment(Negative)) />
              <CommentList comments=(filterBySentiment(Hateful)) />
            </div>;
          }
        )
      </div>,
  };
};
