[%bs.raw {|require('./Comment.css')|}];

type state = {rejected: bool};

type actions =
  | Reject;

let component = ReasonReact.reducerComponent("Comment");

let sentimentToClass = (sentiment: CommentData.Sentiment.t) =>
  switch sentiment {
  | Hateful => {js|😡|js}
  | Negative => {js|😠|js}
  | Neutral => {js|😐|js}
  | Positive => {js|🙂|js}
  };

let make = (~comment: CommentData.Comment.t, _children) => {
  let sentiment = sentimentToClass(CommentData.Sentiment.sentiment(comment));
  let reject = (_event, {ReasonReact.send}) => {
    comment |> CommentData.reject((() => send(Reject)) |> Callback.ignoreError)
  };
  {
    ...component,
    initialState: () => {rejected: false},
    reducer: (action, _state) =>
      switch action {
      | Reject => ReasonReact.Update({rejected: true})
      },
    render: ({ReasonReact.state, handle}) =>
      <div className="Comment">
        <div className="Comment__sentiment">
          (ReasonReact.stringToElement(sentiment))
        </div>
        <div className="Comment__body">
          (ReasonReact.stringToElement(comment.body))
        </div>
        <div className="Comment__actions">
          (
            state.rejected ?
              ReasonReact.stringToElement("Rejected") :
              <button onClick=(handle(reject))>
                (ReasonReact.stringToElement("Reject"))
              </button>
          )
        </div>
      </div>
  };
};
