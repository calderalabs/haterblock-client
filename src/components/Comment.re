[%bs.raw {|require('./Comment.css')|}];

let component = ReasonReact.statelessComponent("Comment");

let sentimentToClass = (sentiment: CommentData.Sentiment.t) =>
  switch sentiment {
  | Hateful => "hateful"
  | Negative => "negative"
  | Neutral => "neutral"
  | Positive => "positive"
  };

let make = (~comment: CommentData.Comment.t, _children) => {
  let sentiment = sentimentToClass(CommentData.Sentiment.sentiment(comment));
  {
    ...component,
    render: _self =>
      <div className={j|Comment Comment--$sentiment|j}>
        <div className="body">
          CommentData.(ReasonReact.stringToElement(comment.body))
        </div>
      </div>
  };
};
