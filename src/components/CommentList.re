open Belt;

[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: list(CommentData.Comment.t),
  markedForRejection: list(Model.id),
};

type action =
  | Reject(list(CommentData.Comment.t), unit => unit)
  | ToggleForRejection(CommentData.Comment.t);

let component = ReasonReact.reducerComponent("CommentList");

let make = (~comments: list(CommentData.Comment.t), _children) => {
  let reject =
      (
        comment: CommentData.Comment.t,
        {ReasonReact.send},
        callback: Callback.t(unit, unit),
      ) =>
    comment
    |> CommentData.reject(response =>
         switch (response) {
         | Success () => send(Reject([comment], () => callback(response)))
         | Error () => callback(response)
         }
       );
  let rejectMarked = (self, callback: Callback.t(unit, unit)) => {
    let markedForRejection = self.ReasonReact.state.markedForRejection;
    markedForRejection
    |> CommentData.rejectAll(response =>
         switch (response) {
         | Success () =>
           self.send(
             Reject(
               CommentData.Comment.forIds(
                 self.state.comments,
                 markedForRejection,
               ),
               () => callback(response),
             ),
           )
         | Error () => callback(response)
         }
       );
  };
  let isMarkedForRejection =
      (markedForRejection: list(Model.id), comment: CommentData.Comment.t) =>
    markedForRejection |> List.has(_, comment.id, (==));
  {
    ...component,
    initialState: () => {comments, markedForRejection: []},
    reducer: (action, state) =>
      switch (action) {
      | Reject(rejectedComments, callback) =>
        let updateComment = comment =>
          isMarkedForRejection(state.markedForRejection, comment) ?
            {...comment, status: Rejected} : comment;
        let updatedComments = rejectedComments |> List.map(_, updateComment);
        ReasonReact.UpdateWithSideEffects(
          {...state, comments: updatedComments},
          (_self => callback()),
        );
      | ToggleForRejection(comment) =>
        let markedForRejection =
          isMarkedForRejection(state.markedForRejection, comment) ?
            state.markedForRejection |> List.keep(_, id => comment.id != id) :
            [comment.id, ...state.markedForRejection];
        ReasonReact.Update({...state, markedForRejection});
      },
    render: self =>
      <div className="CommentList">
        <div className="CommentList__header">
          <div>
            <AsyncButton onClick=(rejectMarked(self))>
              (ReasonReact.stringToElement("Reject Marked"))
            </AsyncButton>
          </div>
        </div>
        <div className="CommentList__comments">
          (
            self.state.comments
            |> List.map(_, comment =>
                 CommentData.Comment.(
                   <div key=(string_of_int(comment.id))>
                     <Comment comment onReject=(reject(comment, self))>
                       <input
                         name="markedForRejection"
                         _type="checkbox"
                         checked=(
                           Js.Boolean.to_js_boolean(
                             isMarkedForRejection(
                               self.state.markedForRejection,
                               comment,
                             ),
                           )
                         )
                         onChange=(
                           _event => self.send(ToggleForRejection(comment))
                         )
                       />
                     </Comment>
                   </div>
                 )
               )
            |> List.toArray
            |> ReasonReact.arrayToElement
          )
        </div>
      </div>,
  };
};
