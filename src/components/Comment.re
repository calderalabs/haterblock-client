[%bs.raw {|require('./Comment.css')|}];

let component = ReasonReact.statelessComponent("Comment");

let sentimentToString = (sentiment: CommentData.sentiment) =>
  switch sentiment {
  | Hateful => "Hateful"
  | Negative => "Negative"
  | Neutral => "Neutral"
  | Positive => "Positive"
  };

let make = (~comment: CommentData.t, _children) => {
  ...component,
  render: _self =>
    <div className="Comment">
      <div className="body">
        CommentData.(ReasonReact.stringToElement(comment.body))
      </div>
      <div className="sentiment">
        CommentData.(
          ReasonReact.stringToElement(sentimentToString(sentiment(comment)))
        )
      </div>
    </div>
};
