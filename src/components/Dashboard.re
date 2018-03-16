[%bs.raw {|require('./Dashboard.css')|}];

type state = {comments: option(array(CommentData.Comment.t))};

type actions =
  | CommentsLoaded(array(CommentData.Comment.t));

let component = ReasonReact.reducerComponent("Dashboard");

let comments: array(CommentData.Comment.t) = [|
  {id: 1, body: "I don't like this", score: (-6), videoId: 0},
  {id: 2, body: "This is not so great but ok", score: (-3), videoId: 0},
  {id: 3, body: "Awful content mate", score: (-5), videoId: 0},
  {id: 4, body: "This is fucking shit", score: (-8), videoId: 0},
  {id: 5, body: "You should kill yourself", score: (-7), videoId: 0},
  {id: 6, body: "This is total garbage", score: (-9), videoId: 0},
  {id: 7, body: "This is really good", score: 5, videoId: 0}
|];

let make = (~user: UserData.User.t, _children) => {
  let loadComments = ({ReasonReact.send}) => send(CommentsLoaded(comments));
  {
    ...component,
    initialState: () => {comments: None},
    reducer: (action, _state: state) =>
      switch action {
      | CommentsLoaded(comments) =>
        ReasonReact.Update({comments: Some(comments)})
      },
    didMount: self => {
      loadComments(self);
      ReasonReact.NoUpdate;
    },
    render: ({state}) =>
      <div className="Dashboard">
        <div> (ReasonReact.stringToElement(string_of_int(user.id))) </div>
        (
          switch state.comments {
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
      </div>
  };
};
