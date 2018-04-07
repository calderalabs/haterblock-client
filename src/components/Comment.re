open Belt;

[%bs.raw {|require('./Comment.css')|}];

open MomentRe;

let component = ReasonReact.statelessComponent("Comment");

let sentimentToEmoji = (sentiment: CommentData.Sentiment.t) =>
  switch (sentiment) {
  | Negative => {js|😠|js}
  | Neutral => {js|😐|js}
  | Positive => {js|🙂|js}
  };

let make =
    (
      ~comment: CommentData.Comment.t,
      ~onReject: Callback.action(unit, unit),
      ~checked: bool,
      ~onChange: unit => unit,
      _children,
    ) => {
  let sentiment = sentimentToEmoji(comment.sentiment);
  let rejected = comment.status == Rejected;
  {
    ...component,
    render: _self =>
      <div className="Comment">
        <div className="Comment__bulkActions">
          <input
            name="markedForRejection"
            _type="checkbox"
            checked=(Js.Boolean.to_js_boolean(checked))
            disabled=(Js.Boolean.to_js_boolean(rejected))
            onChange=(_event => onChange())
          />
        </div>
        <div className="Comment__sentiment">
          (ReasonReact.stringToElement(sentiment))
        </div>
        <div className="Comment__body">
          <div className="Comment__publishedAt">
            (
              ReasonReact.stringToElement(
                Moment.fromNow(comment.publishedAt, ~withoutSuffix=None),
              )
            )
          </div>
          <div dangerouslySetInnerHTML={"__html": comment.body} />
        </div>
        <div className="Comment__actions">
          (
            comment.status == Rejected ?
              ReasonReact.stringToElement("Rejected") :
              <AsyncButton onClick=onReject className="Button Button--small">
                (ReasonReact.stringToElement("Reject"))
              </AsyncButton>
          )
        </div>
      </div>,
  };
};
