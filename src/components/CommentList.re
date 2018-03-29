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
        <div className="CommentList__header">
          (
            switch (self.state.comments) {
            | None
            | Some([]) => ReasonReact.nullElement
            | Some(_) =>
              <div className="CommentList__bulkActions">
                <span
                  className="fa fa-arrow-down CommentList__bulkActionsIcon"
                />
                <AsyncButton
                  onClick=(rejectMarked(self))
                  className="Button Button--small">
                  (ReasonReact.stringToElement("Reject Marked"))
                </AsyncButton>
              </div>
            }
          )
          <div className="CommentList__title">
            (
              ReasonReact.stringToElement(
                CommentData.Sentiment.toString(sentiment),
              )
            )
          </div>
          <div>
            (
              switch (self.state.comments) {
              | None
              | Some([]) => ReasonReact.nullElement
              | Some(comments) =>
                let totalEntries = self.state.totalEntries;
                let count = comments |> List.length;
                <div className="CommentList__nav">
                  <div className="CommentList__count">
                    (
                      ReasonReact.stringToElement(
                        {j|Showing $(count) of $(totalEntries)|j},
                      )
                    )
                  </div>
                  <ReactPaginate
                    pageCount=self.state.totalPages
                    pageRangeDisplayed=1
                    marginPagesDisplayed=1
                    pageClassName="CommentList__paginationItem"
                    containerClassName="CommentList__pagination"
                    previousClassName="CommentList__paginationItem"
                    nextClassName="CommentList__paginationItem"
                    previousLinkClassName="CommentList__paginationLink"
                    nextLinkClassName="CommentList__paginationLink"
                    activeClassName="CommentList__paginationItem--active"
                    pageLinkClassName="CommentList__paginationLink"
                    breakClassName="CommentList__paginationItem"
                    previousLabel=<span className="fa fa-chevron-left" />
                    nextLabel=<span className="fa fa-chevron-right" />
                    onPageChange=(
                      data => loadComments(self, ~page=data##selected + 1)
                    )
                  />
                </div>;
              }
            )
          </div>
        </div>
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
                               _event =>
                                 self.send(ToggleForRejection(comment))
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
          }
        )
      </div>,
  };
};
