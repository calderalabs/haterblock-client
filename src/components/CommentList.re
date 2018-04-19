open Belt;

[%bs.raw {|require('./CommentList.css')|}];

type state = {
  comments: list(CommentData.Comment.t),
  response: option(CommentData.response),
  markedForRejection: list(Model.id),
  filters: CommentListFilters.filters,
};

type action =
  | Reject(list(CommentData.Comment.t), unit => unit)
  | ToggleForRejection(CommentData.Comment.t)
  | CommentsLoaded(CommentData.response)
  | ToggleAllForRejection
  | SetFilters(CommentListFilters.filters);

let component = ReasonReact.reducerComponent("CommentList");

let make = _children => {
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
    | [] => callback(Success())
    | comments =>
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
  let loadComments = (~page=1, {ReasonReact.send, state}) =>
    CommentData.fetchAll(
      ~status=state.filters.status,
      ~sentiment=state.filters.sentiment,
      ~page,
      response =>
      send(CommentsLoaded(response))
    );
  {
    ...component,
    initialState: () => {
      comments: [],
      response: None,
      markedForRejection: [],
      filters: {
        status: [Published],
        sentiment: [Hateful, Negative],
      },
    },
    reducer: (action, state) =>
      switch (action) {
      | Reject(rejectedComments, callback) =>
        let updateComment = comment =>
          rejectedComments |> List.has(_, comment, (==)) ?
            {...comment, status: Rejected} : comment;
        let updatedComments = state.comments |> List.map(_, updateComment);
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
      | ToggleAllForRejection =>
        let publishedComments =
          CommentData.Comment.publishedComments(state.comments);
        let allCommentsMarkedForRejection =
          List.length(publishedComments)
          == List.length(state.markedForRejection);
        let markedForRejection =
          if (allCommentsMarkedForRejection) {
            [];
          } else {
            publishedComments
            |> List.reduce(_, state.markedForRejection, (acc, comment) =>
                 isMarkedForRejection(state.markedForRejection, comment) ?
                   acc : List.add(acc, comment.id)
               );
          };
        ReasonReact.Update({...state, markedForRejection});
      | CommentsLoaded(response) =>
        switch (response) {
        | Success(document) =>
          ReasonReact.Update({
            ...state,
            response: Some(response),
            comments: document.resources,
          })
        | Error () =>
          ReasonReact.Update({
            ...state,
            response: Some(response),
            comments: [],
          })
        }
      | SetFilters(filters) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, comments: [], response: None, filters},
          (self => loadComments(self)),
        )
      },
    didMount: self => {
      loadComments(self);
      ReasonReact.NoUpdate;
    },
    render: self =>
      switch (self.state.response) {
      | Some(Error ()) => ReasonReact.nullElement
      | _ =>
        let (totalEntries, totalPages) =
          switch (self.state.response) {
          | Some(Success(document)) => (
              document.meta.totalEntries,
              document.meta.totalPages,
            )
          | _ => (0, 0)
          };
        <div className="CommentList">
          <CommentListHeader
            comments=self.state.comments
            onRejectMarked=(rejectMarked(self))
            onPageChange=(page => loadComments(self, ~page))
            totalEntries
            totalPages
            markedForRejection=self.state.markedForRejection
            onSelectAll=(() => self.send(ToggleAllForRejection))
          />
          <CommentListFilters
            filters=self.state.filters
            onFiltersChange=(filters => self.send(SetFilters(filters)))
          />
          (
            switch (self.state.comments, self.state.response) {
            | (_, None) =>
              <div className="CommentList__emptyComments">
                <MessageBox>
                  (ReasonReact.stringToElement("Loading..."))
                </MessageBox>
              </div>
            | ([], _) =>
              <div className="CommentList__emptyComments">
                <MessageBox>
                  (
                    ReasonReact.stringToElement(
                      "There are no comments to display",
                    )
                  )
                </MessageBox>
              </div>
            | (comments, _) =>
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
                             checked=(
                               isMarkedForRejection(
                                 self.state.markedForRejection,
                                 comment,
                               )
                             )
                             onChange=(
                               () => self.send(ToggleForRejection(comment))
                             )
                           />
                         </div>
                       )
                     )
                  |> List.toArray
                  |> ReasonReact.arrayToElement
                )
              </div>
            | _ => ReasonReact.nullElement
            }
          )
        </div>;
      },
  };
};
