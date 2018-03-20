open Belt;

[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: list(CommentData.Comment.t),
  markedForRejection: list(int)
};

type action =
  | Reject(CommentData.Comment.t, unit => unit)
  | ToggleForRejection(CommentData.Comment.t);

let component = ReasonReact.reducerComponent("CommentList");

let make = (~comments: list(CommentData.Comment.t), _children) => {
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
    |> List.keep(_, comment =>
         List.has(self.state.markedForRejection, comment.CommentData.Comment.id, (==))
       )
    |> List.map(_, comment =>
         comment
         |> CommentData.reject(response =>
              switch response {
              | Success () => self.send(Reject(comment, () => ()))
              | _ => ()
              }
            )
       )
    |> ignore;
  let isMarkedForRejection =
      (markedForRejection: list(int), comment: CommentData.Comment.t) =>
    markedForRejection |> List.has(_, comment.id, (==));
  {
    ...component,
    initialState: () => {comments, markedForRejection: []},
    reducer: (action, state) =>
      switch action {
      | Reject(rejectedComment, callback) =>
        let updateComment = comment =>
          comment == rejectedComment ?
            {...comment, rejected: true} : comment;
        let updatedComments = state.comments |> List.map(_, updateComment);
        ReasonReact.UpdateWithSideEffects(
          {...state, comments: updatedComments},
          (_self => callback())
        );
      | ToggleForRejection(comment) =>
        let markedForRejection = state.markedForRejection;
        let markedForRejection =
          isMarkedForRejection(markedForRejection, comment) ?
            markedForRejection |> List.keep(_, id => comment.id != id) :
            [comment.id, ...markedForRejection];
        ReasonReact.Update({...state, markedForRejection});
      },
    render: self =>
      <div className="CommentList">
        <div className="Comments">
          (
            self.state.comments
            |> List.map(_, comment =>
                 CommentData.Comment.(
                   <div key=(string_of_int(comment.id))>
                     <Comment comment onReject=(reject(comment, self)) />
                     <input
                       name="markedForRejection"
                       _type="checkbox"
                       checked=(
                         Js.Boolean.to_js_boolean(
                           isMarkedForRejection(
                             self.state.markedForRejection,
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
            |>
            List.toArray |>
            ReasonReact.arrayToElement
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
