[%bs.raw {|require('./Comment.css')|}];

let component = ReasonReact.statelessComponent("Dasboard");

let make = (~comment: CommentData.t, _children) => {
  ...component,
  render: _self =>
    <div className="Comment">
      CommentData.(ReasonReact.stringToElement(comment.body))
    </div>
};
