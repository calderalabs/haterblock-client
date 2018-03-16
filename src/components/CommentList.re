[%bs.raw {|require('./CommentList.css')|}];

let component = ReasonReact.statelessComponent("CommentList");

let make = (~comments: array(CommentData.Comment.t), _children) => {
  ...component,
  render: _self =>
    <div className="CommentList">
      (
        comments
        |> Array.map(comment =>
             CommentData.Comment.(
               <div key=(string_of_int(comment.id))> <Comment comment /> </div>
             )
           )
        |> ReasonReact.arrayToElement
      )
    </div>
};
