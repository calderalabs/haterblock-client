open Belt;

[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: option(list(CommentData.Comment.t)),
  totalPages: int,
  totalEntries: int,
  markedForRejection: list(Model.id),
};

type action =
  | Reject(list(CommentData.Comment.t), unit => unit)
  | ToggleForRejection(CommentData.Comment.t)
  | CommentsLoaded(JsonApi.Document.decodedMany(CommentData.Comment.t));

let component = ReasonReact.reducerComponent("CommentList");

let make = (~sentiment: CommentData.Sentiment.t, _children) => {
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
  let rejectMarked = (self, callback: Callback.t(unit, unit)) =>
    switch (self.ReasonReact.state.comments) {
    | None
    | Some([]) => callback(Success())
    | Some(comments) =>
      let markedForRejection = self.ReasonReact.state.markedForRejection;
      markedForRejection
      |> CommentData.rejectAll(response =>
           switch (response) {
           | Success () =>
             self.send(
               Reject(
                 CommentData.Comment.forIds(comments, markedForRejection),
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
  let loadComments = (~page=1, {ReasonReact.send}) =>
    CommentData.fetchAll(~sentiment, ~page, response =>
      switch (response) {
      | Success(document) => send(CommentsLoaded(document))
      | Error () => ()
      }
    );
  {
    ...component,
    initialState: () => {
      comments: None,
      markedForRejection: [],
      totalPages: 0,
      totalEntries: 0,
    },
    reducer: (action, state) =>
      switch (action) {
      | Reject(rejectedComments, callback) =>
        switch (state.comments) {
        | Some(comments) =>
          let updateComment = comment =>
            rejectedComments |> List.has(_, comment, (==)) ?
              {...comment, status: Rejected} : comment;
          let updatedComments = comments |> List.map(_, updateComment);
          ReasonReact.UpdateWithSideEffects(
            {...state, comments: Some(updatedComments)},
            (_self => callback()),
          );
        | None => ReasonReact.NoUpdate
        }
      | ToggleForRejection(comment) =>
        let markedForRejection =
          isMarkedForRejection(state.markedForRejection, comment) ?
            state.markedForRejection |> List.keep(_, id => comment.id != id) :
            [comment.id, ...state.markedForRejection];
        ReasonReact.Update({...state, markedForRejection});
      | CommentsLoaded(document) =>
        ReasonReact.Update({
          ...state,
          comments: Some(document.resources),
          totalPages: document.meta.totalPages,
          totalEntries: document.meta.totalEntries,
        })
      },
    didMount: self => {
      loadComments(self);
      ReasonReact.NoUpdate;
    },
    render: self =>
      <div className="CommentList">
        <CommentListHeader
          comments=self.state.comments
          onRejectMarked=(rejectMarked(self))
          onPageChange=(page => loadComments(self, ~page))
          totalEntries=self.state.totalEntries
          totalPages=self.state.totalPages
          markedForRejection=self.state.markedForRejection
          sentiment
        />
        (
          switch (self.state.comments) {
          | None =>
            <div className="CommentList__emptyComments">
              <div className="CommentList__emptyMessage">
                (ReasonReact.stringToElement("Loading..."))
              </div>
            </div>
          | Some([]) =>
            <div className="CommentList__emptyComments">
              <div className="CommentList__emptyMessage">
                (
                  ReasonReact.stringToElement(
                    "There are no comments to display",
                  )
                )
              </div>
            </div>
          | Some(comments) =>
            <div className="CommentList__comments">
              (
                comments
                |> List.map(_, comment =>
                     CommentData.Comment.(
                       <div
                         key=(string_of_int(comment.id))
                         className="CommentList__commentWrapper">
                         <Comment
                           comment
                           onReject=(reject(comment, self))
                           checked=(isMarkedForRejection(self.state.markedForRejection, comment))
                           onChange=(() => self.send(ToggleForRejection(comment)))
                         />
                       </div>
                     )
                   )
                |> List.toArray
                |> ReasonReact.arrayToElement
              )
            </div>
          }
        )
      </div>
  };
};
