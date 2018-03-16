[%bs.raw {|require('./Comment.css')|}];

let component = ReasonReact.statelessComponent("CommentList");

let make = (~comments: array(CommentData.t), _children) => {
  ...component,
  render: _self =>
    <div className="CommentList">
      (
        comments
        |> Array.map(comment =>
             CommentData.(
               <div key=(string_of_int(comment.id))> <Comment comment /> </div>
             )
           )
        |> ReasonReact.arrayToElement
      )
    </div>
};
