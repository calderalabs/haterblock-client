[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: array(CommentData.Comment.t),
  marked_for_rejection: list(int)
};

type action =
  | Reject(CommentData.Comment.t, unit => unit)
  | ToggleForRejection(CommentData.Comment.t);

let component = ReasonReact.reducerComponent("CommentList");

let make = (~comments: array(CommentData.Comment.t), _children) => {
  let reject =
      (
        comment: CommentData.Comment.t,
        {ReasonReact.send},
        callback: Callback.t(unit, unit)
      ) =>
    comment
    |> CommentData.reject(response =>
         switch response {
         | Success () => send(Reject(comment, () => callback(response)))
         | _ => callback(response)
         }
       );
  let rejectMarked = (_event, self) =>
    self.ReasonReact.state.comments
    |> Array.to_list
    |> List.filter(comment =>
         List.exists(id => comment.CommentData.Comment.id == id, self.state.marked_for_rejection)
       )
    |> List.map(comment => comment
      |> CommentData.reject(response =>
          switch response {
          | Success () => self.send(Reject(comment, () => ()))
          | _ => ()
          }
        )
      ) |> ignore;
  let isMarkedForRejection =
      (markedForRejection: list(int), comment: CommentData.Comment.t) =>
    markedForRejection |> List.exists(id => comment.id == id);
  {
    ...component,
    initialState: () => {comments, marked_for_rejection: []},
    reducer: (action, state) =>
      switch action {
      | Reject(rejectedComment, callback) =>
        let updateComment = comment =>
          CommentData.Comment.(
            comment.id == rejectedComment.id ? {...comment, rejected: true} : comment
          );
        let updatedComments = state.comments |> Array.map(updateComment);
        ReasonReact.UpdateWithSideEffects(
          {...state, comments: updatedComments},
          (_self => callback())
        );
      | ToggleForRejection(comment) => {
          let markedForRejection = state.marked_for_rejection;
          let markedForRejection = isMarkedForRejection(markedForRejection, comment) ?
            markedForRejection |> List.filter(id => comment.id != id) :
            [comment.id, ...markedForRejection];
          ReasonReact.Update({...state, marked_for_rejection: markedForRejection});
        }
      },
    render: self =>
      <div className="CommentList">
        <div className="Comments">
          (
            self.state.comments
            |> Array.map(comment =>
                 CommentData.Comment.(
                   <div key=(string_of_int(comment.id))>
                     <Comment comment onReject=(reject(comment, self)) />
                     <input
                       name="markedForRejection"
                       _type="checkbox"
                       checked=(
                         Js.Boolean.to_js_boolean(
                           isMarkedForRejection(
                             self.state.marked_for_rejection,
                             comment
                           )
                         )
                       )
                       onChange=(
                         _event => self.send(ToggleForRejection(comment))
                       )
                     />
                   </div>
                 )
               )
            |> ReasonReact.arrayToElement
          )
        </div>
        <div className="Actions">
          <button onClick=(self.handle(rejectMarked))>
            (ReasonReact.stringToElement("Reject Marked"))
          </button>
        </div>
      </div>
  };
};
