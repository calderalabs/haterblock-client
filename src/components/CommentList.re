[%bs.raw {|require('./CommentList.css')|}];

type state = {comments: array(CommentData.Comment.t)};

type action =
  | Reject(CommentData.Comment.t, (unit => unit));

let component = ReasonReact.reducerComponent("CommentList");

let make = (~comments: array(CommentData.Comment.t), _children) => {
  let reject =
      (
        comment: CommentData.Comment.t,
        {ReasonReact.send},
        callback: Callback.t(unit, unit)
      ) =>
    comment
    |> CommentData.reject(response => {
         switch response {
         | Success () => send(Reject(comment, () => callback(response)))
         | _ => callback(response)
         };
       });
  {
    ...component,
    initialState: () => {comments: comments},
    reducer: (action, state) =>
      switch action {
      | Reject(rejectedComment, callback) =>
        let updateComment = comment =>
          switch comment {
          | comment when comment == rejectedComment => {...comment, rejected: true}
          | _ => comment
          };
        let updatedComments =
          (
            state.comments
            |> Array.map(updateComment)
          );
        ReasonReact.UpdateWithSideEffects(
          {comments: updatedComments},
          (_self) => callback()
        );
      },
    render: self =>
      <div className="CommentList">
        (
          self.state.comments
          |> Array.map(comment =>
               CommentData.Comment.(
                 <div key=(string_of_int(comment.id))>
                   <Comment comment onReject=(reject(comment, self)) />
                 </div>
               )
             )
          |> ReasonReact.arrayToElement
        )
      </div>
  };
};
