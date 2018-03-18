[%bs.raw {|require('./Comment.css')|}];

type state = {rejected: bool};

type action =
  | Reject;

let component = ReasonReact.reducerComponent("Comment");

let sentimentToEmoji = (sentiment: CommentData.Sentiment.t) =>
  switch sentiment {
  | Hateful => {js|😡|js}
  | Negative => {js|😠|js}
  | Neutral => {js|😐|js}
  | Positive => {js|🙂|js}
  };

let make = (~comment: CommentData.Comment.t, _children) => {
  let sentiment = sentimentToEmoji(CommentData.Sentiment.sentiment(comment));
  let reject = ({ReasonReact.send}, callback: Callback.t(unit, unit)) =>
    comment
    |> CommentData.reject(
        (
          (response) => {
            switch response {
            | Success() => send(Reject)
            | _ => ()
            };
            callback(response);
          }
        )
      );
  {
    ...component,
    initialState: () => {rejected: false},
    reducer: (action, _state) =>
      switch action {
      | Reject => ReasonReact.Update({rejected: true})
      },
    render: (self) =>
      <div className="Comment">
        <div className="Comment__sentiment">
          (ReasonReact.stringToElement(sentiment))
        </div>
        <div className="Comment__body">
          (ReasonReact.stringToElement(comment.body))
        </div>
        <div className="Comment__actions">
          (
            self.state.rejected ?
              ReasonReact.stringToElement("Rejected") :
              <AsyncButton onClick=reject(self)>
                (ReasonReact.stringToElement("Reject"))
              </AsyncButton>
          )
        </div>
      </div>
  };
};
