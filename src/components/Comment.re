open Belt;

[%bs.raw {|require('./Comment.css')|}];

let component = ReasonReact.statelessComponent("Comment");

let sentimentToEmoji = (sentiment: CommentData.Sentiment.t) =>
  switch (sentiment) {
  | Hateful => {js|😡|js}
  | Negative => {js|😠|js}
  | Neutral => {js|😐|js}
  | Positive => {js|🙂|js}
  };

let make =
    (
      ~comment: CommentData.Comment.t,
      ~onReject: Callback.action(unit, unit),
      children,
    ) => {
  let sentiment = sentimentToEmoji(CommentData.Sentiment.sentiment(comment));
  {
    ...component,
    render: _self =>
      <div className="Comment">
        (
          ReasonReact.createDomElement(
            "div",
            ~props={"className": "Comment__bulkActions"},
            children,
          )
        )
        <div className="Comment__sentiment">
          (ReasonReact.stringToElement(sentiment))
        </div>
        <div className="Comment__body">
          (ReasonReact.stringToElement(comment.body))
        </div>
        <div className="Comment__actions">
          (
            comment.rejected ?
              ReasonReact.stringToElement("Rejected") :
              <AsyncButton onClick=onReject>
                (ReasonReact.stringToElement("Reject"))
              </AsyncButton>
          )
        </div>
      </div>,
  };
};
