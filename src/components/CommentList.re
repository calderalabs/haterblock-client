open Belt;

[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: list(CommentData.Comment.t),
  markedForRejection: list(int)
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
        callback: Callback.t(unit, unit)
      ) =>
    comment
    |> CommentData.reject(response =>
         switch response {
         | Success () => send(Reject([comment], () => callback(response)))
         | Error () => callback(response)
         }
       );
  let rejectMarked = (self, callback: Callback.t(unit, unit)) => {
    let markedForRejection = self.ReasonReact.state.markedForRejection;

    markedForRejection |> CommentData.rejectAll(response =>
      switch response {
      | Success () => self.send(Reject(CommentData.Comment.forIds(self.state.comments, markedForRejection), () => callback(response)))
      | Error () => callback(response)
      }
    );
  };
  let isMarkedForRejection =
      (markedForRejection: list(int), comment: CommentData.Comment.t) =>
    markedForRejection |> List.has(_, comment.id, (==));
  {
    ...component,
    initialState: () => {comments, markedForRejection: []},
    reducer: (action, state) =>
      switch action {
      | Reject(rejectedComments, callback) =>
        let updateComment = comment =>
          List.has(state.markedForRejection, comment.CommentData.Comment.id, (==)) ?
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
          <AsyncButton onClick=(rejectMarked(self))>
            (ReasonReact.stringToElement("Reject Marked"))
          </AsyncButton>
        </div>
      </div>
  };
};
